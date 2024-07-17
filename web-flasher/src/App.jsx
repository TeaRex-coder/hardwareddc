import React from "react";
import EspWebInstallButton from "./EspWebInstallButton";

function App() {
  return (
    <div id="root">
      <div className="main-content">
        <h1>Flash HardwareDDC</h1>
        <p>
          This tool will install HardwareDDC. Plug in your device and click the
          button to flash:
        </p>
        <EspWebInstallButton />
      </div>
      <footer>
        <p>
          <a
            href="https://github.com/TeaRex-coder/hardwareddc"
            target="_blank"
            rel="noopener noreferrer"
          >
            GitHub
          </a>{" "}
          |
          <a
            href="https://hardwareddc.alexandreboutoille.com"
            target="_blank"
            rel="noopener noreferrer"
          >
            {" "}
            Website
          </a>
        </p>
      </footer>
    </div>
  );
}

export default App;
