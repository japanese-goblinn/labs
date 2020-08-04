import animate from "../scripts/animate.js";
import Database from "../scripts/database.js";
import FolderComponent from "./components/folderComponent.js";

export default class FoldersColumnView {

    #body = async () => /*html*/`
        <nav>
            <ul id="folder-list" class="list folder-list"></ul>
        </nav>
        <button id="folder-create-trigger" class="primary-button">+ New folder</button>
    `

    #configure = async () => {
        const folderCreate = document.getElementById('folder-create-trigger');
        animate(folderCreate, 'primary-button-click-animation');
    }

    async _loadData() {
        const folderList = document.getElementById('folder-list');
        const folders = await Database.loadAllFolders();
        for (const folder of folders) {
            const folderView = new FolderComponent(
                folderList, 
                folder.id, 
                folder.title, 
                folder.description
            );
            await folderView.render();
        }
    }

    async render() {
        this.container.innerHTML = await this.#body();
        await this._loadData();
        await this.#configure();
    }

    constructor(container) {
        const containerName = container.className;
        this.container = container;
        this.container.classList.remove(...this.container.classList);
        this.container.classList.add(containerName);
    }
}
