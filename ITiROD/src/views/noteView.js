import { mobile, router, renderer } from "../app.js";
import Database from "../scripts/database.js";
import NoteMoveView from "./noteMoveView.js";

export default class NoteView {

    #body = async (id, content, date) => /*html*/`
        <button id="note-back-button" class="back-button note-back-button selectable">
            <img src="../../../assets/back.svg" alt="Back" />
        </button>
        <time datetime="2017-1-3 15:00-0800">${date.toLocaleDateString('en-US') + ' ' + date.toLocaleTimeString('en-US')}</time>
        <div id="note-toolbar" class="toolbar">
            <button id="toolbar-note-move-${id}" class="toolbar-item white-background selectable">
                <img src="../../../assets/folder.svg" alt="Move to folder" />
            </button>
            <button id="toolbar-note-delete-${id}" class="toolbar-item selectable">
                <img src="../../../assets/trash.svg" alt="Delete" />
            </button>
        </div>
        <div id="markdown-toolbar" class="toolbar">
            <button id="markdown-toolbar-item-write" class="toolbar-item selectable">Write</button>
            <button id="markdown-toolbar-item-preview" class="toolbar-item selectable">Preview</button>
        </div>
        <div class="markdown-area">
            <textarea id="markdown-input" class="markdown-presentable"
                placeholder="Your markdown here...">${content}</textarea>
            <div id="markdown-preview" class="markdown-presentable"></div>
        </div>
    `

    #configure = async (noteID, folderID) => {
        const noteBack = document.getElementById('note-back-button');
        noteBack.addEventListener('click', () => window.history.back());
        
        const textarea = document.getElementById('markdown-input');
        const preview = document.getElementById('markdown-preview');
        textarea.addEventListener('input', () => this._handleMarkdown(textarea, preview));
        this._handleMarkdown(textarea, preview);
    
        let noteConent = textarea.value;
        textarea.addEventListener('focusout', async () => {
            if (noteConent.replace(/\s+$/, '') === textarea.value.replace(/\s+$/, '')) {
                return;
            }
            noteConent = textarea.value;
            await Database.updateNote(folderID, noteID, noteConent);
            if (mobile.matches) {
                return;
            }
            await renderer.render('NotesColumnView');
            await renderer.render('NoteView');
        });

        const noteDelete = document.getElementById(`toolbar-note-delete-${noteID}`);
        noteDelete.addEventListener('click', async () => {
            const deletedConfirmed = confirm(`Are you shure want to delete this note?`);
            if (!deletedConfirmed) {
                return;
            }
            await Database.deleteNote(folderID, noteID);
            await router.navigate('folder/' + folderID);
        });

        const container = document.getElementById('note-move-container');
        window.addEventListener('click', (event) => {
            if (event.target != container) {
                return;
            }
            container.style.display = 'none';
        });
        const moveNote = document.getElementById(`toolbar-note-move-${noteID}`);
        moveNote.addEventListener('click', async () => {
            const noteMoveModal = new NoteMoveView(container, noteID, folderID);
            await noteMoveModal.render();
            container.style.display = 'block';
        });
    
        const markdownWriteActivate = document.getElementById('markdown-toolbar-item-write');
        const markdownPreviewActivate = document.getElementById('markdown-toolbar-item-preview');
        markdownWriteActivate.addEventListener('click', () => {
            textarea.style.display = 'inline-block';
            preview.style.display = 'none';
            markdownWriteActivate.style.backgroundColor = 'var(--blur-color)';
            markdownPreviewActivate.style.backgroundColor = 'white';
        });
        markdownPreviewActivate.addEventListener('click', () => {
            preview.style.display = 'inline-block';
            textarea.style.display = 'none';
            markdownPreviewActivate.style.backgroundColor = 'var(--blur-color)';
            markdownWriteActivate.style.backgroundColor = 'white';
        });
        this._mediaQueryMatch(mobile);
    }

    _mediaQueryMatch(viewConfig) {
        const noteToolbar = document.getElementById('note-toolbar');
        const markdownToolbar = document.getElementById('markdown-toolbar');
        const noteBack = document.getElementById('note-back-button');
        const textarea = document.getElementById('markdown-input');
        const preview = document.getElementById('markdown-preview');
        const markdownWriteActivate = document.getElementById('markdown-toolbar-item-write');
        if (viewConfig.matches) {
            noteToolbar.style.width = '100%';
            markdownToolbar.style.display = 'flex';
            noteBack.style.display = 'inline-block';
            textarea.classList.add('markdown-presentable-mobile');
            preview.classList.add('markdown-presentable-mobile');
            markdownWriteActivate.click();
        } else {
            noteToolbar.style.width = '25%';
            markdownToolbar.style.display = 'none';
            noteBack.style.display = 'none';
            textarea.classList.add('markdown-presentable-desktop');
            preview.classList.add('markdown-presentable-desktop');
        }
    }

    _handleMarkdown(inputSource, previewSource) {
        previewSource.innerHTML = marked(inputSource.value);
    }

    async _loadNote() {
        const data = router.dataFromURL();
        const noteID = data.noteID;
        const folderID = data.folderID;
        const note = await Database.loadNote(folderID, noteID);
        return note
    }

    async render() {
        const note = await this._loadNote();
        this.container.innerHTML = await this.#body(note.id, note.content, new Date(note.date));
        await this.#configure(note.id, note.folderID);
    }

    constructor(container) {
        const matchMobile = window.matchMedia('screen and (max-width: 800px)');
        if (matchMobile.matches) {
            this.container = document.getElementById('folders-column-container');
            this.container.classList.remove(...this.container.classList);
            this.container.classList.add(container.className);
        } else {
            this.container = container;
        }
    }
}
