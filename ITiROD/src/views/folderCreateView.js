import FolderComponent from "./components/folderComponent.js";
import Database from "../scripts/database.js";

export default class FolerCreateView {

    #body = async () => /*html*/`
        <div class="modal-content">
            <div id="folder-create-form" class="form-container">
                <form class="form">
                    <label for="folder-name">Folder name</label>
                    <input id="folder-name" type="text" placeholder="New Folder" />
                    <label for="folder-description">Folder desctiption</label>
                    <input id="folder-description" type="text" placeholder="Some fancy description (max 30 symbols)" maxlength="30"/>
                    <button id="confirm-new-folder-button" class="primary-button" type="button">Create</button>
                </form>
            </div>
        </div>  
    `
    #configure = async () => {
        const folderList = document.getElementById('folder-list');
        const createFolder = document.getElementById('confirm-new-folder-button');
        const folderNameInput = document.getElementById('folder-name');
        const folderDescriptionInput = document.getElementById('folder-description');

        createFolder.addEventListener('click', async () => {
            if (!folderNameInput.value) {
                folderNameInput.value = 'New Folder';
            }
            const folderID = await Database.saveFolder(folderNameInput.value, folderDescriptionInput.value);
            const newFolder = new FolderComponent(
                folderList, 
                folderID,
                folderNameInput.value, 
                folderDescriptionInput.value
            );
            await newFolder.render();
            this.container.style.display = 'none';
            folderNameInput.value = '';
            folderDescriptionInput.value = '';
        });
    }

    async render() {
        this.container.innerHTML = await this.#body();
        await this.#configure();
    }

    constructor(container) {
        this.container = container;
    }
}
