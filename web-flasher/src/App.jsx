import ModalComponent from "./components/modal";
import EspWebInstallButton from "./components/EspWebInstallButton";

function App() {
  return (
    <div className="flex flex-col min-h-screen bg-neutral-50 text-neutral-800 dark:bg-neutral-950 dark:text-neutral-50">
      <div className="flex-grow flex flex-col items-center justify-center">
        <h1 className="text-5xl font-extrabold text-center mt-6 mb-12">
          Flash HardwareDDC
        </h1>
        <p className="text-center mb-4">
          This tool will install HardwareDDC. Plug in your device and click the
          button to flash:
        </p>
        <EspWebInstallButton />
      </div>

      <footer className="mt-8 mb-16">
        <div className="flex justify-center space-x-4">
          <a
            href="https://github.com/TeaRex-coder/hardwareddc"
            target="_blank"
            rel="noopener noreferrer"
            className="text-cyan-700 cursor-pointer"
          >
            Github
          </a>
          <span className="mx-2">|</span>
          <span className="text-cyan-700 cursor-pointer">
            <ModalComponent />
          </span>
        </div>
      </footer>
    </div>
  );
}

export default App;
