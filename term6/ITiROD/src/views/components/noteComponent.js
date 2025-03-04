import { router } from "../../app.js";
import Database from "../../scripts/database.js";
import NoteMoveView from "../noteMoveView.js";

export default class NoteComponent {

    #body = async (id, title) => /*html*/`
        <li id="${id}" class="dropdown selectable" draggable='true'>
            <div class="note-description">
                <h5>${title}</h5>
            </div>
            <button id="show-note-dropdown-${id}" class="note-button rounded">
                ...
            </button>
            <div id="note-dropdown-${id}" class="dropdown-container">
                <button id="note-move-${id}" class="secondary selectable">Move to</button>
                <button id="note-delete-${id}" class="destructive selectable">Delete</button>
            </div>
        </li>
    `
    
    #configure = async () => {
        
        const showDropdown = document.getElementById(`show-note-dropdown-${this.id}`);
        const dropdown = document.getElementById(`note-dropdown-${this.id}`);
        showDropdown.addEventListener('click', () => {
            dropdown.style.display = 'flex';
            dropdown.style.flexDirection = 'column';
        });
        window.addEventListener('click', (event) => {
            if (event.target === dropdown || event.target === showDropdown) {
                return;
            }
            dropdown.style.display = 'none';
        });

        const deleteNote = document.getElementById(`note-delete-${this.id}`);
        deleteNote.addEventListener('click', async () => {
            const deletedConfirmed = confirm(`Are you shure want to delete this note?`);
            if (!deletedConfirmed) {
                return;
            }
            await Database.deleteNote(this.folderID, this.id);
            await router.navigate('folder/' + this.folderID);
        });

        const container = document.getElementById('note-move-container');
        window.addEventListener('click', (event) => {
            if (event.target != container) {
                return;
            }
            container.style.display = 'none';
        });
        const moveNote = document.getElementById(`note-move-${this.id}`);
        moveNote.addEventListener('click', async () => {
            const noteMoveModal = new NoteMoveView(container, this.id, this.folderID);
            await noteMoveModal.render();
            container.style.display = 'block';
        });

        const noteListElement = document.getElementById(`${this.id}`);
        noteListElement.addEventListener('click', async (event) => {
            if (event.target === moveNote || event.target === deleteNote || event.target === showDropdown) {
                return;
            }
            await router.navigate('note/' + `${this.id}`);
        });

        noteListElement.addEventListener('dragstart', (event) => {
            const dataString = JSON.stringify({
                id: event.target.id,
                folderID: this.folderID,
                noteID: this.id
            });
            event.dataTransfer.setData('text/plain', dataString);
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
