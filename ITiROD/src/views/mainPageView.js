import { renderer } from '../app.js';

export default class MainPageView {

    #body = async () => /*html*/`
        <div id="profile-container" class="profile-container modal"></div>
        <div id="search-container" class="search-container modal"></div>
        <div id="folder-create-container" class="modal"></div>
        <div id="folder-edit-container" class="modal"></div>
        <div class="root-layout">
            <header id='header-container' class="layout-header"></header>
            <div class="main-columns">
                <div id="folders-column-container" class="folders-column"></div>
                <div id="notes-column-container" class="notes-column"></div>
                <main id="note-content-container" class="note-view-column"></main>
            </div>
        </div>
    `

    #configure = async () => {
        document.title = 'WebNotes';
        document.styleSheets[0].disabled = false;
        document.styleSheets[1].disabled = true;
        
        await renderer.render('HeaderView');
        await renderer.render('FoldersColumnView');

        await renderer.render('ProfileView');
        await renderer.render('FolderCreateView');
        await renderer.render('SearchView');
    }

    async render() {
        this.container.innerHTML = await this.#body();
        await this.#configure();
    }

    constructor(container) {
        this.container = container;
    }
}
