import React, { useEffect } from "react";
import "./App.css";

const EspWebInstallButton = () => {
  useEffect(() => {
    const script = document.createElement("script");
    script.src = "installer/install-button.js";
    script.type = "module";
    script.async = true;
    document.body.appendChild(script);

    return () => {
      document.body.removeChild(script);
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
