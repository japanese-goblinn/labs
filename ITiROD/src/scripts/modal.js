export default function handleModalWindow(modal, triggerButton) {
    triggerButton.addEventListener('click', () => {
        modal.style.display = "block";
    });

    window.addEventListener('click', (event) => {
        if (event.target != modal) {
            return
        }
        modal.style.display = "none";
    })
}
