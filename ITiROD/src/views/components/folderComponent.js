import { router } from "../../app.js";

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
                <button class="secondary selectable">Edit</button>
                <button class="destructive selectable">Delete</button>
            </div>
        </li>
    `
    #configure = async () => {
        const folderListElement = document.getElementById(`${this.id}`);
        folderListElement.addEventListener('click', async () => {
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
