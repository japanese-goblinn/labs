import handleMarkdown from '../../scripts/markdown.js';

export default class NoteView {

    #body = async () => /*html*/`
        <time datetime="2017-1-3 15:00-0800">2017-1-3 15:00</time>
        <div class="toolbar">
            <button class="toolbar-item selectable">
                <img src="../../../assets/folder.svg" alt="Move to folder" />
            </button>
            <button class="toolbar-item selectable">
                <img src="../../../assets/trash.svg" alt="Delete" />
            </button>
        </div>
        <div>
            <textarea id="markdown-input" class="markdown-presentable"
                placeholder="Your markdown here..."></textarea>
            <div id="markdown-preview" class="markdown-presentable"></div>
        </div>
    `

    async render() {
        this.container.innerHTML = await this.#body();
        const textarea = document.getElementById('markdown-input');
        const preview = document.getElementById('markdown-preview');
        handleMarkdown(textarea, preview);
    }

    constructor(container) {
        this.container = container;
    }
}
