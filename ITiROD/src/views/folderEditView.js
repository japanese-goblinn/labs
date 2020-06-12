import Database from "../scripts/database.js";
import { router, renderer, mobile } from "../app.js";

export default class FolerEditView {

    #body = async (id, title, desciption) => /*html*/`
        <div class="modal-content">
            <div id="folder-edit-form-${id}" class="form-container">
                <form id="edit-form-${id}" class="form">
                    <label for="edit-folder-name-${id}">New folder name</label>
                    <input id="edit-folder-name-${id}" type="text" 
                        placeholder="New name (max 50 symbols)" value="${title}" maxlength="50" required />
                    <label for="edit-folder-description-${id}">New folder desctiption</label>
                    <input id="edit-folder-description-${id}" type="text" 
                        placeholder="Some fancy description (max 100 symbols)" value="${desciption}" maxlength="100" />
                    <button id="confirm-update-folder-button-${id}" class="primary-button modal-folder-button">
                        Update
                    </button>
                </form>
            </div>
        </div>  
    `

    #configure = async () => {
        const newFolderDescription = document.getElementById(`edit-folder-description-${this.id}`);
        const newFolderTitle = document.getElementById(`edit-folder-name-${this.id}`);

        const form = document.getElementById(`edit-form-${this.id}`);
        form.addEventListener('submit', async (event) => {
            event.preventDefault();

            await Database.editFolder(this.id, newFolderTitle.value, newFolderDescription.value);
            this.container.style.display = 'none';
            if (mobile.matches) {
                await renderer.render('FoldersColumnView');
                return;
            }
            await router.refresh();
        });
    }

    async render() {
        this.container.innerHTML = await this.#body(this.id, this.title, this.description);
        await this.#configure();
    }

    constructor(container, data) {
        this.container = container;
        this.id = data.id;
        this.title = data.title;
        this.description = data.description;
    }
}