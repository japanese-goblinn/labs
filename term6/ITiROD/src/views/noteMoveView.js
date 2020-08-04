import Database from "../scripts/database.js";
import { router, renderer, mobile } from "../app.js";

export default class NoteMoveView {

    #body = async (id) => /*html*/`
        <div class="modal-content">
            <div class="form-container">
                <form class="form">
                    <label for="move-note-select-${id}">Move to folder</label>
                    <select id="move-note-select-${id}"></select>
                    <button id="move-note-confirm-${id}" class="primary-button modal-folder" type="button">
                        Move
                    </button>
                </form>
            </div>
        </div>  
    `
    #configure = async () => {
        const folders = await Database.loadAllFolders();
        const select = document.getElementById(`move-note-select-${this.id}`);
        const confirmMove = document.getElementById(`move-note-confirm-${this.id}`);
        if (folders.length == 1 && folders[0].id == this.folderID) {
            select.insertAdjacentHTML('beforeend', `<option>No folders available</option>`);
            confirmMove.disabled = 'disabled';
            confirmMove.style.background = 'var(--blur-color)';
            return;
        } 
        for (const folder of folders) {
            if (folder.id == this.folderID) {
                continue;
            }
            select.insertAdjacentHTML('beforeend', `<option value="${folder.id}">${folder.title}</option>`);
        }
        confirmMove.addEventListener('click', async () => {
            const newFolderID = select.options[select.selectedIndex].value;
            await Database.moveNote(this.id, this.folderID, newFolderID);
            await router.navigate('folder/' + this.folderID);
            const container = document.getElementById('note-move-container');
            container.style.display = 'none';
        });
    }

    async render() {
        this.container.innerHTML = await this.#body(this.id);
        await this.#configure();
    }

    constructor(container, id, folderID) {
        this.container = container;
        this.id = id;
        this.folderID = folderID;
    }
}