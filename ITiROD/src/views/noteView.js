import { mobile } from '../app.js';

export default class NoteView {

    #body = async () => /*html*/`
        <button id="note-back-button" class="back-button note-back-button selectable">
            <img src="../../../assets/back.svg" alt="Back" />
        </button>
        <time datetime="2017-1-3 15:00-0800">2017-1-3 15:00</time>
        <div id="note-toolbar" class="toolbar">
            <button class="toolbar-item white-background selectable">
                <img src="../../../assets/folder.svg" alt="Move to folder" />
            </button>
            <button class="toolbar-item selectable">
                <img src="../../../assets/trash.svg" alt="Delete" />
            </button>
        </div>
        <div id="markdown-toolbar" class="toolbar">
            <button id="markdown-toolbar-item-write" class="toolbar-item selectable">Write</button>
            <button id="markdown-toolbar-item-preview" class="toolbar-item selectable">Preview</button>
        </div>
        <div class="markdown-area">
            <textarea id="markdown-input" class="markdown-presentable"
                placeholder="Your markdown here..."># Some text</textarea>
            <div id="markdown-preview" class="markdown-presentable"></div>
        </div>
    `

    #configure = async () => {
        const noteBack = document.getElementById('note-back-button');
        noteBack.addEventListener('click', () => window.history.back());
        
        const textarea = document.getElementById('markdown-input');
        const preview = document.getElementById('markdown-preview');
        textarea.addEventListener('input', () => this._handleMarkdown(textarea, preview));
        this._handleMarkdown(textarea, preview);

        const markdownWriteActivate = document.getElementById('markdown-toolbar-item-write');
        const markdownPreviewActivate = document.getElementById('markdown-toolbar-item-preview');
        markdownWriteActivate.addEventListener('click', () => {
            textarea.style.display = "inline-block";
            preview.style.display = "none";
            markdownWriteActivate.style.backgroundColor = "var(--blur-color)";
            markdownPreviewActivate.style.backgroundColor = "white";
        });
        markdownPreviewActivate.addEventListener('click', () => {
            preview.style.display = "inline-block";
            textarea.style.display = "none";
            markdownPreviewActivate.style.backgroundColor = "var(--blur-color)";
            markdownWriteActivate.style.backgroundColor = "white";
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
            noteToolbar.style.width = "100%";
            markdownToolbar.style.display = "flex";
            noteBack.style.display = "inline-block";
            textarea.style.maxHeight = "67vh";
            preview.style.maxHeight = "67vh";
            markdownWriteActivate.click();
        } else {
            noteToolbar.style.width = "25%";
            markdownToolbar.style.display = "none";
            noteBack.style.display = "none";
            textarea.style.maxHeight = "77vh";
            preview.style.maxHeight = "77vh";
        }
    }

    _handleMarkdown(inputSource, previewSource) {
        previewSource.innerHTML = marked(inputSource.value);
    }

    async render() {
        this.container.innerHTML = await this.#body();
        await this.#configure();
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
