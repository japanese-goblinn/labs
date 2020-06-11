import { router } from "../../app.js";

export default class NoteComponent {

    #body = async (id, title) => /*html*/`
        <li id="${id}" class="dropdown selectable">
            <div class="note-description">
                <h5>${title}</h5>
            </div>
            <button class="note-button rounded dropdown-trigger">
                ...
            </button>
            <div class="dropdown-container">
                <button class="secondary selectable">Move to</button>
                <button class="destructive selectable">Delete</button>
            </div>
        </li>
    `
    
    #configure = async () => {
        const noteListElement = document.getElementById(`${this.id}`);
        noteListElement.addEventListener('click', async () => {
            await router.navigate('note/' + `${this.id}`);
        });
    }

    async render() {
        this.container.insertAdjacentHTML(
            'beforeend', 
            await this.#body(this.id, this.title)
        );
        await this.#configure();
    }

    constructor(container, id, title) {
        this.container = container;
        this.id = id;
        this.title = title;
    }
}
