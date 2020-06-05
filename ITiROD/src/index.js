import './profile/profile.js';
import './search/search.js';
import handleModalWindow from './scripts/modal.js';
import animate from './scripts/animate.js';
import handleMarkdown from './scripts/markdown.js';
import Router from './scripts/router.js';

function configureViews() {
    const textarea = document.getElementById('markdown-input');
    const preview = document.getElementById('markdown-preview');
    handleMarkdown(textarea, preview);

    const buttons = document.getElementsByClassName('primary-button');
    Array.from(buttons).forEach((button) => {
        animate(button, 'primary-button-click-animation');
    });
    
    const newFolderButton = document.getElementById('new-folder-button');
    const folderModal = document.getElementById('folder-modal');
    handleModalWindow(folderModal, newFolderButton);
}

configureViews();
