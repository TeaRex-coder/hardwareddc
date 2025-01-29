import "./App.css";

function App() {
  const hotkeys = [
    { keys: "Alt+Shift+-", action: "Brightness down 10%" },
    { keys: "Alt+Shift+=", action: "Brightness up 10%" },
    { keys: "Alt+Shift+1", action: "Set brightness to 25%" },
    { keys: "Alt+Shift+2", action: "Set brightness to 50%" },
    { keys: "Alt+Shift+3", action: "Set brightness to 75%" },
    { keys: "Alt+Shift+4", action: "Set brightness to 100%" },
    { keys: "Alt+Shift+5", action: "Set brightness to 0%" },

    { keys: "Alt+Shift+d+1", action: "Set input source to DisplayPort 1" },
    { keys: "Alt+Shift+d+2", action: "Set input source to DisplayPort 2" },
    { keys: "Alt+Shift+h+1", action: "Set input source to HDMI 1" },
    { keys: "Alt+Shift+h+2", action: "Set input source to HDMI 2" },
  ];

  return (
    <div id="app">
      <ul>
        {hotkeys.map((hotkey, index) => (
          <li key={index}>
            <strong>{hotkey.keys}</strong>: {hotkey.action}
          </li>
        ))}
      </ul>
    </div>
  );
}

export default App;
