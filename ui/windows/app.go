package main

import (
	"context"
	"fmt"
	"io"
	"net/http"
	"strconv"

	hook "github.com/robotn/gohook"
)

type App struct {
	ctx context.Context
}

func NewApp() *App {
	return &App{}
}

func (a *App) startup(ctx context.Context) {
	a.ctx = ctx
	go a.startKeyListener()
}

func (a *App) startKeyListener() {
	// Brightness
	hook.Register(hook.KeyDown, []string{"alt", "shift", "1"}, func(e hook.Event) {
		a.setBrightness(25)
	})

	hook.Register(hook.KeyDown, []string{"alt", "shift", "2"}, func(e hook.Event) {
		a.setBrightness(50)
	})

	hook.Register(hook.KeyDown, []string{"alt", "shift", "3"}, func(e hook.Event) {
		a.setBrightness(75)
	})

	hook.Register(hook.KeyDown, []string{"alt", "shift", "4"}, func(e hook.Event) {
		a.setBrightness(100)
	})

	hook.Register(hook.KeyDown, []string{"alt", "shift", "5"}, func(e hook.Event) {
		a.setBrightness(0)
	})

	hook.Register(hook.KeyDown, []string{"alt", "shift", "-"}, func(e hook.Event) {
		currentBrightness := a.getBrightness()
		newBrightness := currentBrightness - 10
		if newBrightness < 0 {
			newBrightness = 0
		}
		a.setBrightness(newBrightness)
	})

	hook.Register(hook.KeyDown, []string{"alt", "shift", "="}, func(e hook.Event) {
		currentBrightness := a.getBrightness()
		newBrightness := currentBrightness + 10
		if newBrightness > 100 {
			newBrightness = 100
		}
		a.setBrightness(newBrightness)
	})

	// Input sources
	hook.Register(hook.KeyDown, []string{"alt", "shift", "d", "1"}, func(e hook.Event) {
		a.setSource("0x0f")
	})

	hook.Register(hook.KeyDown, []string{"alt", "shift", "d", "2"}, func(e hook.Event) {
		a.setSource("0x10")
	})

	hook.Register(hook.KeyDown, []string{"alt", "shift", "h", "1"}, func(e hook.Event) {
		a.setSource("0x11")
	})

	hook.Register(hook.KeyDown, []string{"alt", "shift", "h", "2"}, func(e hook.Event) {
		a.setSource("0x12")
	})

	s := hook.Start()
	for range hook.Process(s) {
	}
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
