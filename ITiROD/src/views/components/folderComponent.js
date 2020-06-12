import { router } from "../../app.js";
import Database from "../../scripts/database.js";
import FolderEditView from "../folderEditView.js";
import debounce from "../../scripts/debounce.js";

export default class FolderComponent {

    #body = async (id, title, description) => /*html*/`
        <li id="${id}" class="dropdown selectable">
            <div class="folder-description">
                <h4>${title}</h4>
                <p class="secondary" style="${description ? '' : 'display: none;'}">
                    ${description}
                </p>
            </div>
            <button class="folder-button rounded selectable dropdown-trigger">
                ...
            </button>
            <div id="actions-dropdown-${id}" class="dropdown-container">
                <button id="edit-${id}" class="secondary selectable">Edit</button>
                <button id="delete-${id}" class="destructive selectable">Delete</button>
            </div>
        </li>
    `
    #configure = async () => {
        const actionsModal = document.getElementById(`actions-dropdown-${this.id}`);
        const editFolder = document.getElementById(`edit-${this.id}`);
        editFolder.addEventListener('click', async () => {
            const container = document.getElementById('folder-edit-container');
            const folderEditView = new FolderEditView(container, {
                id: this.id,
                title: this.title,
                description: this.description
            });
            await folderEditView.render();
            container.style.display = 'block';
            window.addEventListener('click', (event) => {
                if (event.target != container) {
                    return
                }
                container.style.display = 'none';
            })
            actionsModal.style.pointerEvents = "none";
            setTimeout(() => actionsModal.style.pointerEvents = "auto", 50);
        });

        const deleteFolder = document.getElementById(`delete-${this.id}`);
        deleteFolder.addEventListener('click', async () => {
            const deletedConfirmed = confirm(`Are you shure want to delete '${this.title}'?`);
            if (!deletedConfirmed) {
                return;
            }
            await Database.deleteFolder(this.id, this.title);
            await router.navigate('/');
        });

        const folderListElement = document.getElementById(`${this.id}`);
        folderListElement.addEventListener('click', debounce(async (event) => {
            if (event.target === editFolder || event.target === deleteFolder) {
                return;
            }
            await router.navigate('folder/' + `${this.id}`);
        }, 150));
        
    }

    async render() {
        this.container.insertAdjacentHTML(
            'beforeend', 
            await this.#body(this.id, this.title, this.description)
        );
        await this.#configure();
    }

    constructor(container, id, title, description) {
        this.container = container;
        this.title = title;
        this.description = description;
        this.id = id;
    }
}
