import handleModalWindow from './scripts/modal.js';

import HeaderView from './views/headerView.js';
import FoldersColumnView from './views/foldersColumnView.js';
import NotesColumnView from './views/notesColumnView.js';
import NoteView from './views/noteView.js';
import ProfileView from './views/profileView.js';
import SearchView from './views/searchView.js';
import FolderCreateView from './views/folderCreateView.js';
import MainPageView from './views/mainPageView.js';
import WelcomePageView from './views/welcomePageView.js';
import NotFounded from './views/404.js';

export default class Renderer {

    #views = {
        'MainPageView': {
            class: MainPageView,
            containerID: 'root'
        },
        'WelcomePageView': {
            class: WelcomePageView,
            containerID: 'root'
        },
        '404': {
            class: NotFounded,
            containerID: 'root'
        },
        'HeaderView': {
            class: HeaderView,
            containerID: 'header-container'
        },
        'NotesColumnView': {
            class: NotesColumnView,
            containerID: 'notes-column-container'
        },
        'FoldersColumnView': {
            class: FoldersColumnView,
            containerID: 'folders-column-container'
        },
        'NoteView': {
            class: NoteView,
            containerID: 'note-content-container'
        },
        'ProfileView': {
            class: ProfileView,
            containerID: 'profile-container',
            activateButtonID: 'profile-trigger'
        },
        'SearchView': {
            class: SearchView,
            containerID: 'search-container',
            activateButtonID: 'search-trigger'
        },
        'FolderCreateView': {
            class: FolderCreateView,
            containerID: 'folder-create-container',
            activateButtonID: 'folder-create-trigger'
        }
    };

    async renderEmpty(viewName) {
        const viewObject = this.#views[viewName];
        if (!viewObject) {
            console.log(`No view with name ${viewName}`);
            return
        }
        const viewContainer = document.getElementById(viewObject.containerID);
        viewContainer.innerHTML = null;
    }

    async render(viewName, data = null) {
        const viewObject = this.#views[viewName];
        if (!viewObject) {
            console.log(`No view with name ${viewName}`);
            return
        }
        const viewContainer = document.getElementById(viewObject.containerID);
        const view = new viewObject.class(viewContainer);
        await view.render();
        if (viewObject.activateButtonID) {
            const activateButton = document.getElementById(viewObject.activateButtonID);
            handleModalWindow(viewContainer, activateButton);
        } 
    }
}
