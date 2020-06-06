import handleModalWindow from '../../scripts/modal.js';
import animate from '../../scripts/animate.js';

import HeaderView from './headerView.js';
import FoldersColumnView from './foldersColumnView.js';
import NotesColumnView from './notesColumnView.js';
import NoteView from './noteView.js';
import ProfileView from './profileView.js';
import SearchView from './searchView.js';
import FolderCreateView from './folderCreateView.js';

export default class IndexRenderer {

    #body = async () => /*html*/`
        <div id="profile-container" class="profile-container modal"></div>
        <div id="search-container" class="search-container modal"></div>
        <div id="folder-create-container" class="modal"></div>
        <div class="root-layout">
            <header id='header-container' class="layout-header"></header>
            <div class="main-columns">
                <div id="folders-column-container" class="folders-column"></div>
                <div id="notes-column-container" class="notes-column"></div>
                <main id="note-content-container"></main>
            </div>
        </div>
    `
    
    async _renderModal(className, containerID, acrivationButtonId) {
        const viewContainer = document.getElementById(containerID);
        const view = new className(viewContainer);
        await view.render();
        const activateButton = document.getElementById(acrivationButtonId);
        handleModalWindow(viewContainer, activateButton);
    }

    async _renderView(className, containerID) {
        const viewContainer = document.getElementById(containerID);
        const view = new className(viewContainer);
        await view.render();
    }

    async render() {
        document.getElementById('root').innerHTML = await this.#body();

        await this._renderView(HeaderView, 'header-container');
        await this._renderView(FoldersColumnView, 'folders-column-container');
        await this._renderView(NotesColumnView, 'notes-column-container');
        await this._renderView(NoteView, 'note-content-container');

        await this._renderModal(ProfileView, 'profile-container', 'profile-trigger');
        await this._renderModal(FolderCreateView, 'folder-create-container', 'folder-create-trigger');
        await this._renderModal(SearchView, 'search-container', 'search-trigger');
        
        const buttons = document.getElementsByClassName('primary-button');
        Array.from(buttons).forEach((button) => {
            animate(button, 'primary-button-click-animation');
        });
    }
}
