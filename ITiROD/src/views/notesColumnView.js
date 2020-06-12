import animate from "../../scripts/animate.js";
import { router, renderer, mobile } from "../app.js";
import NoteComponent from "./components/noteComponent.js";
import Database from "../scripts/database.js";

export default class NotesColumnView {

    #body = async (title, description) => /*html*/`
        <button id="folder-back-button" class="back-button folder-back-button selectable">
            <img src="../../../assets/back.svg" alt="Back" />
        </button>
        <section class="notes-header">
            <div class="notes-header-description">
                <h3>${title}</h3>
                <p class="secondary" style="${description ? '' : 'display: none;'}">
                    ${description}
                </p>
            </div>
            <button id="new-note-button" class="primary-button rounded">+</button>
        </section>
        <hr />
        <nav>
            <ul id="note-list" class="list note-list"></ul>
        </nav>
    `

    #configure = async (folderID) => {
;        const noteList = document.getElementById('note-list');

        const folderBack = document.getElementById('folder-back-button');
        folderBack.addEventListener('click', () => window.history.back());
        folderBack.style.display = "none";

        this.container.classList.add('right-bordered');
        
        await renderer.renderEmpty('NoteView');

        const newNoteButton = document.getElementById('new-note-button');
        animate(newNoteButton, 'primary-button-click-animation');
        newNoteButton.addEventListener('click', async () => {
            const noteTitle = 'New Note'
            const date = Date.now();
            const noteID = await Database.saveNote(folderID, noteTitle, date);
            const newNote = new NoteComponent(noteList, folderID, noteID, noteTitle);
            await newNote.render();
        });

        if (mobile.matches) {
            folderBack.style.display = 'inline-block';
        } else {
            folderBack.style.display = 'none';
        }
    }

    async _loadFolder() {
        const folderID = router.dataFromURL().folderID;
        const folder = await Database.loadFolder(folderID);
        return folder;
    }

    async _loadNotes(folderID) {
        const noteList = document.getElementById('note-list');
        const notes = await Database.loadAllNotes(folderID);
        for (const note of notes) {
            const noteView = new NoteComponent(
                noteList, 
                folderID,
                note.id, 
                note.content, 
            );
            await noteView.render();
        }
    }

    async render() {
        const folder = await this._loadFolder();
        this.container.innerHTML = await this.#body(folder.title, folder.description);
        await this.#configure(folder.id);
        await this._loadNotes(folder.id);
    }

    constructor(container) {
        if (mobile.matches) {
            this.container = document.getElementById('folders-column-container');
            this.container.classList.remove(...this.container.classList);
            this.container.classList.add(container.className);
        } else {
            this.container = container;
        }
    }
}
