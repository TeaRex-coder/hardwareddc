import {
  Modal,
  ModalTrigger,
  ModalBody,
  ModalContent,
  ModalFooter,
} from "./animated-modal";

const ModalComponent = () => {
  return (
    <div>
      <Modal>
        <ModalTrigger>
          <button className="btn-primary">Purchase</button>
        </ModalTrigger>

        <ModalBody>
          <ModalContent className="flex flex-col justify-center items-center text-center h-full space-y-4 p-6">
            <p className="text-xl font-semibold text-neutral-800 dark:text-neutral-50">
              HardwareDDC
            </p>
            <p className="text-lg font-medium text-neutral-800 dark:text-neutral-50">
              CA$100
            </p>
            <p className="text-sm text-neutral-800 dark:text-neutral-50">
              Currently shipping to Canada, America the UK, & Belgium. If you
              need another country, please contact me on either:
            </p>
            <div className="flex space-x-4">
              <a
                href="https://twitter.alexandreboutoille.com"
                target="_blank"
                rel="noopener noreferrer"
                className="text-neutral-950 hover:text-cyan-700 dark:text-neutral-50 transition duration-300 ease-in-out"
              >
                Twitter
              </a>
              <a
                href="https://discord.alexandreboutoille.com"
                target="_blank"
                rel="noopener noreferrer"
                className="text-neutral-950 hover:text-cyan-700 dark:text-neutral-50 transition duration-300 ease-in-out"
              >
                Discord
              </a>
              <a
                href="mailto:alexandre@boutoille.com"
                className="text-neutral-950 hover:text-cyan-700 dark:text-neutral-50 transition duration-300 ease-in-out"
              >
                Email
              </a>
            </div>
            <a
              href="https://buy.stripe.com/fZebL5b21fJ0chOfZ1"
              target="_blank"
              rel="noopener noreferrer"
              className="install-button rounded-lg border-2 border-transparent px-4 py-2 text-base font-medium bg-neutral-950 text-neutral-50 hover:bg-cyan-900 hover:text-neutral-50 hover:border-cyan-600 dark:bg-neutral-50 dark:text-neutral-950 transition duration-300 ease-in-out cursor-pointer"
            >
              Buy
            </a>
          </ModalContent>
          <ModalFooter> </ModalFooter>
        </ModalBody>
      </Modal>
    </div>
  );
};

export default ModalComponent;
