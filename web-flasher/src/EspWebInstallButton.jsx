import React, { useEffect } from "react";
import "./App.css";

const EspWebInstallButton = () => {
  useEffect(() => {
    const loadScript = async () => {
      await import("./installer/install-button.js");
    };

    loadScript();

    return () => {
      // Any cleanup if necessary
    };
  }, []);

  return (
    <div className="main">
      <esp-web-install-button manifest="./manifest.json">
        <button className="btn" slot="activate">
          Install
        </button>
      </esp-web-install-button>
    </div>
  );
};

export default EspWebInstallButton;
