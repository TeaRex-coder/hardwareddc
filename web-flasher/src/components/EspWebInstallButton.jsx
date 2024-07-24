import React, { useEffect } from "react";

const EspWebInstallButton = () => {
  useEffect(() => {
    const loadScript = async () => {
      await import("../installer/install-button.js");
    };

    loadScript();
  }, []);

  return (
    <div className="main">
      <esp-web-install-button manifest="./manifest.json">
        <button
          slot="activate"
          className="install-button rounded-lg border-2 border-transparent px-4 py-2 text-base font-medium bg-neutral-950 text-neutral-50 hover:bg-cyan-900 hover:text-neutral-50 hover:border-cyan-700  dark:bg-neutral-50 dark:text-neutral-950 transition duration-300 ease-in-out cursor-pointer"
        >
          Install
        </button>
      </esp-web-install-button>
    </div>
  );
};

export default EspWebInstallButton;
