import { router } from "../../app.js";
import Database from "../../scripts/database.js";

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
            <div class="dropdown-container">
                <button id="edit-${id}" class="secondary selectable">Edit</button>
                <button id="delete-${id}" class="destructive selectable">Delete</button>
            </div>
        </li>
    `
    #configure = async () => {
        const editFolder = document.getElementById(`edit-${this.id}`);
        editFolder.addEventListener('click', () => {
            console.log('edit ' + this.id);
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
        folderListElement.addEventListener('click', async (event) => {
            if (event.target === editFolder || event.target === deleteFolder) {
                return;
            }
            await router.navigate('folder/' + `${this.id}`);
        });
        
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
