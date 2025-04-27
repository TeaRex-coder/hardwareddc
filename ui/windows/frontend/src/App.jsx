import { useState, useEffect } from "react";
import { GetHotkeysConfig, ReloadHotkeysAndListener } from "../wailsjs/go/main/App"; // Adjust path if needed
import "./App.css";

function formatHotkeyAction(hotkey) {
  // Return a simplified representation of the action/value
  switch (hotkey.action) {
    case "setBrightness":
    case "setContrast":
      return `${hotkey.value}%`; // Just the percentage
    case "adjustBrightness":
    case "adjustContrast":
      // Return just the value adjustment, e.g., +10% or -10%
      return `${hotkey.value > 0 ? '+' : ''}${hotkey.value}%`;
    case "setSource":
      const sourceMap = {
        "0x0f": "DP 1", // Shortened names
        "0x10": "DP 2",
        "0x11": "HDMI 1",
        "0x12": "HDMI 2",
      };
      return `${sourceMap[hotkey.value] || hotkey.value}`; // Just the source name
    default:
      return `${hotkey.action}`; // Fallback to action name
  }
}

function App() {
  const [brightnessHotkeys, setBrightnessHotkeys] = useState([]);
  const [contrastHotkeys, setContrastHotkeys] = useState([]);
  const [sourceHotkeys, setSourceHotkeys] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);
  const [reloading, setReloading] = useState(false);
  const [reloadStatus, setReloadStatus] = useState(""); // For success/error messages

  const fetchAndGroupHotkeys = async () => {
    try {
      setLoading(true);
      setError(null);
      setReloadStatus(""); // Clear previous reload status
      const result = await GetHotkeysConfig();
      const brightness = [];
      const contrast = [];
      const source = [];

      (result || []).forEach(hk => {
        if (hk.action.includes("Brightness")) {
          brightness.push(hk);
        } else if (hk.action.includes("Contrast")) {
          contrast.push(hk);
        } else if (hk.action.includes("Source")) {
          source.push(hk);
        }
      });

      setBrightnessHotkeys(brightness);
      setContrastHotkeys(contrast);
      setSourceHotkeys(source);

    } catch (err) {
      console.error("Error fetching hotkeys:", err);
      setError("Failed to load hotkey configuration.");
      setBrightnessHotkeys([]);
      setContrastHotkeys([]);
      setSourceHotkeys([]);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchAndGroupHotkeys();
  }, []);

  const handleReload = async () => {
    setReloading(true);
    setReloadStatus("Reloading...");
    setError(null); // Clear previous errors
    try {
      await ReloadHotkeysAndListener();
      setReloadStatus("Hotkeys reloaded successfully!");
      // Re-fetch to update the displayed list
      await fetchAndGroupHotkeys();
    } catch (err) {
      console.error("Error reloading hotkeys:", err);
      setReloadStatus(`Reload failed: ${err}`);
      // Optionally fetch again even on error to reflect the state before the failed reload attempt
      // await fetchAndGroupHotkeys();
    } finally {
      setReloading(false);
      // Optional: Clear the status message after a delay
      setTimeout(() => setReloadStatus(""), 3000);
    }
  };

  const renderHotkeyColumn = (title, hotkeys) => (
    <div className="hotkey-column">
      <h2>{title}</h2>
      {hotkeys.length > 0 ? (
        <ul>
          {hotkeys.map((hotkey, index) => (
            <li key={`${title}-${index}`}>
              <span className="hotkey-value">{formatHotkeyAction(hotkey)}</span>: <strong className="hotkey-keys">{hotkey.keys.join("+")}</strong>
            </li>
          ))}
        </ul>
      ) : (
        <p>No hotkeys defined.</p>
      )}
    </div>
  );

  return (
    <div id="app-container">
      <h1>HardwareDDC Controls</h1>
      {error && <p className="error-message">{error}</p>}
      {loading && <p>Loading hotkeys...</p>}
      {!loading && !error && (
        <>
          <div className="hotkey-grid">
            {renderHotkeyColumn("Brightness", brightnessHotkeys)}
            {renderHotkeyColumn("Contrast", contrastHotkeys)}
            {renderHotkeyColumn("Input Source", sourceHotkeys)}
          </div>
          <div className="reload-section">
            <button onClick={handleReload} disabled={reloading}>
              {reloading ? "Reloading..." : "Reload Hotkeys from File"}
            </button>
            {reloadStatus && <span className="reload-status">{reloadStatus}</span>}
          </div>
        </>
      )}
    </div>
  );
}

export default App;
