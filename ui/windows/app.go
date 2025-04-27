package main

import (
	"context"
	_ "embed" // Required for go:embed
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/http"
	"os"
	"path/filepath" // Needed for joining paths
	"strconv"

	hook "github.com/robotn/gohook"
)

//go:embed hotkeys.json
var defaultHotkeysContent []byte

type App struct {
	ctx context.Context
}

type HotkeyConfig struct {
	Keys   []string    `json:"keys"`
	Action string      `json:"action"`
	Value  interface{} `json:"value"` // Use interface{} for flexible value types (int for brightness, string for source)
}

func NewApp() *App {
	return &App{}
}

func (a *App) startup(ctx context.Context) {
	a.ctx = ctx
	go a.startKeyListener()
}

func (a *App) loadHotkeys() []HotkeyConfig {
	configDir, err := os.UserConfigDir()
	if err != nil {
		log.Printf("Error getting user config directory: %v", err)
		return nil
	}

	appDir := filepath.Join(configDir, "HardwareDDC")
	configPath := filepath.Join(appDir, "hotkeys.json")

	// Check if the config file exists
	if _, err := os.Stat(configPath); os.IsNotExist(err) {
		log.Printf("hotkeys.json not found in %s, creating default.", appDir)
		// Create the directory if it doesn't exist
		if err := os.MkdirAll(appDir, 0755); err != nil {
			log.Printf("Error creating config directory %s: %v", appDir, err)
			return nil
		}
		// Write the default embedded content to the file
		if err := os.WriteFile(configPath, defaultHotkeysContent, 0644); err != nil {
			log.Printf("Error writing default hotkeys.json to %s: %v", configPath, err)
			return nil
		}
	} else if err != nil {
		// Handle other potential errors from os.Stat
		log.Printf("Error checking config file %s: %v", configPath, err)
		return nil
	}

	// Read the config file (either existing or the default we just wrote)
	data, err := os.ReadFile(configPath)
	if err != nil {
		log.Printf("Error reading hotkeys.json from %s: %v", configPath, err)
		return nil
	}

	var configs []HotkeyConfig
	err = json.Unmarshal(data, &configs)
	if err != nil {
		log.Printf("Error parsing hotkeys.json from %s: %v", configPath, err)
		return nil
	}
	log.Printf("Loaded hotkeys from %s", configPath)
	return configs
}

func (a *App) startKeyListener() {
	hotkeys := a.loadHotkeys()
	if hotkeys == nil {
		log.Println("Failed to load hotkeys, listener not started.")
		return
	}

	log.Printf("Registering %d hotkeys...", len(hotkeys))

	for _, hk := range hotkeys {
		// Need to capture the loop variable for the closure
		hk := hk
		hook.Register(hook.KeyDown, hk.Keys, func(e hook.Event) {
			log.Printf("Hotkey triggered: %v, Action: %s, Value: %v", hk.Keys, hk.Action, hk.Value)
			switch hk.Action {
			case "setBrightness":
				if level, ok := hk.Value.(float64); ok { // JSON numbers are often float64
					a.setBrightness(int(level))
				} else {
					log.Printf("Invalid value type for setBrightness: %T", hk.Value)
				}
			case "adjustBrightness":
				if adjustment, ok := hk.Value.(float64); ok {
					currentBrightness := a.getBrightness()
					newBrightness := currentBrightness + int(adjustment)
					if newBrightness < 0 {
						newBrightness = 0
					} else if newBrightness > 100 {
						newBrightness = 100
					}
					a.setBrightness(newBrightness)
				} else {
					log.Printf("Invalid value type for adjustBrightness: %T", hk.Value)
				}
			case "setContrast":
				if level, ok := hk.Value.(float64); ok { // JSON numbers are float64
					a.setContrast(int(level))
				} else {
					log.Printf("Invalid value type for setContrast: %T", hk.Value)
				}
			case "adjustContrast":
				if adjustment, ok := hk.Value.(float64); ok {
					currentContrast := a.getContrast()
					newContrast := currentContrast + int(adjustment)
					if newContrast < 0 {
						newContrast = 0
					} else if newContrast > 100 {
						newContrast = 100
					}
					a.setContrast(newContrast)
				} else {
					log.Printf("Invalid value type for adjustContrast: %T", hk.Value)
				}
			case "setSource":
				if source, ok := hk.Value.(string); ok {
					a.setSource(source)
				} else {
					log.Printf("Invalid value type for setSource: %T", hk.Value)
				}
			default:
				log.Printf("Unknown action: %s", hk.Action)
			}
		})
	}

	log.Println("Hotkey listener starting...")
	s := hook.Start()
	// Keep the process running
	<-hook.Process(s)
	log.Println("Hotkey listener stopped.")
}

// ReloadHotkeysAndListener unregisters existing hotkeys, reloads the config file,
// and registers the new hotkeys. Intended to be called by the frontend.
func (a *App) ReloadHotkeysAndListener() error {
	log.Println("Attempting to reload hotkeys...")

	// 1. Unregister all existing hotkeys
	log.Println("Unregistering all current hotkeys...")
	// hook.UnregisterAll() // Commented out: Function seems unavailable in this gohook version.

	// 2. Reload hotkeys from the configuration file
	log.Println("Reloading hotkey configuration file...")
	configs := a.loadHotkeys() // Re-use the existing load function
	if configs == nil {
		// loadHotkeys already logs errors internally
		log.Println("Failed to reload hotkeys configuration. No new hotkeys registered.")
		// Depending on desired behavior, you might want to return an error here
		// or potentially re-register defaults if the file is now bad.
		// For now, we just stop, leaving no hotkeys active.
		return fmt.Errorf("failed to load hotkey configuration during reload")
	}

	// 3. Register the newly loaded hotkeys
	log.Printf("Registering %d reloaded hotkeys...", len(configs))
	for _, hk := range configs {
		// Need to capture the loop variable for the closure
		hk := hk
		hook.Register(hook.KeyDown, hk.Keys, func(e hook.Event) {
			log.Printf("Hotkey triggered: %v, Action: %s, Value: %v", hk.Keys, hk.Action, hk.Value)
			switch hk.Action {
			case "setBrightness":
				if level, ok := hk.Value.(float64); ok {
					a.setBrightness(int(level))
				} else {
					log.Printf("Invalid value type for setBrightness: %T", hk.Value)
				}
			case "adjustBrightness":
				if adjustment, ok := hk.Value.(float64); ok {
					currentBrightness := a.getBrightness()
					newBrightness := currentBrightness + int(adjustment)
					if newBrightness < 0 {
						newBrightness = 0
					} else if newBrightness > 100 {
						newBrightness = 100
					}
					a.setBrightness(newBrightness)
				} else {
					log.Printf("Invalid value type for adjustBrightness: %T", hk.Value)
				}
			case "setContrast":
				if level, ok := hk.Value.(float64); ok {
					a.setContrast(int(level))
				} else {
					log.Printf("Invalid value type for setContrast: %T", hk.Value)
				}
			case "adjustContrast":
				if adjustment, ok := hk.Value.(float64); ok {
					currentContrast := a.getContrast()
					newContrast := currentContrast + int(adjustment)
					if newContrast < 0 {
						newContrast = 0
					} else if newContrast > 100 {
						newContrast = 100
					}
					a.setContrast(newContrast)
				} else {
					log.Printf("Invalid value type for adjustContrast: %T", hk.Value)
				}
			case "setSource":
				if source, ok := hk.Value.(string); ok {
					a.setSource(source)
				} else {
					log.Printf("Invalid value type for setSource: %T", hk.Value)
				}
			default:
				log.Printf("Unknown action: %s", hk.Action)
			}
		})
	}

	log.Println("Successfully reloaded and registered hotkeys.")
	return nil // Indicate success
}

// GetHotkeysConfig retrieves the current hotkey configuration from the JSON file.
// It's intended to be called by the frontend.
func (a *App) GetHotkeysConfig() []HotkeyConfig {
	configDir, err := os.UserConfigDir()
	if err != nil {
		log.Printf("Error getting user config directory for frontend: %v", err)
		return []HotkeyConfig{} // Return empty on error
	}

	appDir := filepath.Join(configDir, "HardwareDDC")
	configPath := filepath.Join(appDir, "hotkeys.json")

	// Check if the config file exists. If not, create the default.
	// We run this logic here too, in case the app is opened before the listener starts
	// or if the file gets deleted while the app is closed.
	if _, err := os.Stat(configPath); os.IsNotExist(err) {
		log.Printf("hotkeys.json not found in %s for frontend, creating default.", appDir)
		if err := os.MkdirAll(appDir, 0755); err != nil {
			log.Printf("Error creating config directory %s for frontend: %v", appDir, err)
			return []HotkeyConfig{}
		}
		if err := os.WriteFile(configPath, defaultHotkeysContent, 0644); err != nil {
			log.Printf("Error writing default hotkeys.json to %s for frontend: %v", configPath, err)
			return []HotkeyConfig{}
		}
	} else if err != nil {
		log.Printf("Error checking config file %s for frontend: %v", configPath, err)
		return []HotkeyConfig{}
	}

	// Read the config file
	data, err := os.ReadFile(configPath)
	if err != nil {
		log.Printf("Error reading hotkeys.json from %s for frontend: %v", configPath, err)
		return []HotkeyConfig{}
	}

	var configs []HotkeyConfig
	err = json.Unmarshal(data, &configs)
	if err != nil {
		log.Printf("Error parsing hotkeys.json from %s for frontend: %v", configPath, err)
		return []HotkeyConfig{}
	}

	log.Printf("Provided %d hotkeys to frontend from %s", len(configs), configPath)
	return configs
}

// Brightness
func (a *App) getBrightness() int {
	resp, err := http.Get("http://ddcutil.local:3485/1/brightness")
	if err != nil {
		// fmt.Print("\n", err)
		return 50
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		// fmt.Print("\n", err)
		return 50
	}

	brightness, err := strconv.Atoi(string(body))
	if err != nil {
		// fmt.Print("\n", err)
		return 50
	}

	return brightness
}

func (a *App) setBrightness(level int) {
	url := fmt.Sprintf("http://ddcutil.local:3485/1/brightness/%d", level)
	resp, err := http.Get(url)
	if err != nil {
		// fmt.Print("\n", err)
		return
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		// fmt.Print("\n", resp.StatusCode)
	}
}

// Contrast
func (a *App) getContrast() int {
	resp, err := http.Get("http://ddcutil.local:3485/1/contrast")
	if err != nil {
		log.Printf("Error getting contrast: %v", err)
		return 50 // Default value on error
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		log.Printf("Error reading contrast response body: %v", err)
		return 50
	}

	contrast, err := strconv.Atoi(string(body))
	if err != nil {
		log.Printf("Error parsing contrast value: %v", err)
		return 50
	}

	return contrast
}

func (a *App) setContrast(level int) {
	url := fmt.Sprintf("http://ddcutil.local:3485/1/contrast/%d", level)
	resp, err := http.Get(url)
	if err != nil {
		log.Printf("Error setting contrast: %v", err)
		return
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		log.Printf("Error setting contrast: Status %d", resp.StatusCode)
	}
}

// Input source

func (a *App) setSource(source string) {
	url := fmt.Sprintf("http://ddcutil.local:3485/1/input_source/%s", source)
	resp, err := http.Get(url)
	if err != nil {
		// fmt.Print("\n", err)
		return
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		// fmt.Print("\n", resp.StatusCode)
	}
}
