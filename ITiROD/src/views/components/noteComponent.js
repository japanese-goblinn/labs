import { router } from "../../app.js";
import Database from "../../scripts/database.js";

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
                <button id="note-move-${id}" class="secondary selectable">Move to</button>
                <button id="note-delete-${id}" class="destructive selectable">Delete</button>
            </div>
        </li>
    `
    
    #configure = async () => {
        const noteListElement = document.getElementById(`${this.id}`);
        
        const deleteNote = document.getElementById(`note-delete-${this.id}`);
        deleteNote.addEventListener('click', async () => {
            const deletedConfirmed = confirm(`Are you shure want to delete this note?`);
            if (!deletedConfirmed) {
                return;
            }
            await Database.deleteNote(this.folderID, this.id);
            await router.navigate('folder/' + this.folderID);
        });

        const moveNote = document.getElementById(`note-move-${this.id}`);
        moveNote.addEventListener('click', async () => {
            console.log('move');
        });

        noteListElement.addEventListener('click', async (event) => {
            if (event.target === moveNote || event.target === deleteNote) {
                return;
            }
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

    constructor(container, folderID, id, title) {
        this.container = container;
        this.folderID = folderID;
        this.id = id;
        this.title = title;
    }
}
