function handleMarkdown() {
    const textarea = document.getElementById('markdown-input');
    const preview = document.getElementById('markdown-preview');
    textarea.addEventListener('input', () => {
        preview.innerHTML = marked(textarea.value);
    });
}

function animate(button, animationName) {
    button.addEventListener('click', () => {
        button.classList.add(animationName);
    });
    button.addEventListener('animationend', () => {
        button.classList.remove(animationName);
    });
}

function configureButtons() {
    const buttons = document.getElementsByClassName('primary-button');
    Array.from(buttons).forEach((button) => {
        animate(button, 'primary-button-click-animation');
    });
}

handleMarkdown();
configureButtons();

const newFolderButton = document.getElementById('new-folder-button');
const folderModal = document.getElementById('folder-modal');
handleModalWindow(folderModal, newFolderButton);