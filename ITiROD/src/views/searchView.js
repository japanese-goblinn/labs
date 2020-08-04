import debounce from "../scripts/debounce.js";
import Database from "../scripts/database.js";
import NoteSearchComponent from "./components/noteSearchComponent.js";
import FolderSearchComponent from "./components/folderSearchComponent.js";

export default class SearchView {

    #body = async () => /*html*/`
        <div class="modal-content">
            <div class="search">
                <div class="search-bar">
                    <img src="../assets/search.svg" />
                    <input id="search-input" type="search" placeholder="Search..." />
                </div>
                <hr id="search-hr" />
            </div>
            <ul id="search-list" class="list search-list"></ul>
        </div>
    `

    #configure = async () => {
        const dividier = document.getElementById('search-hr');
        dividier.style.display = 'none';
        const searchList = document.getElementById('search-list');

        const search = document.getElementById('search-input');
        search.addEventListener('search', () => {
            if (search.value) {
                return;
            }
            searchList.innerHTML = '';
            dividier.style.display = 'none';
        })
        search.addEventListener('keyup', () => {
            if (search.value) {
                return;
            }
            searchList.innerHTML = '';
            dividier.style.display = 'none';
        })
        search.addEventListener('keyup', debounce(async () => {
            searchList.innerHTML = '';
            dividier.style.display = 'none';
            if (!search.value) {
                return;
            }
            const results = await Database.search(search.value);
            dividier.style.display = 'block';
            if (results.length == 0) {
                searchList.innerHTML = '';
                searchList.insertAdjacentHTML(
                    'beforeend', 
                    `<li class="note-description selectable">
                        <h4 class="secondary">Nothing founded</h4>
                    </li>`
                );
                return;
            }
            for (const res of results) {
                if (res.type === 'note') {
                    const noteView = new NoteSearchComponent(searchList, res.folderID, res.id, res.content);
                    await noteView.render();
                } else {
                    const searchView = new FolderSearchComponent(searchList, res.id, res.title, res.description);
                    await searchView.render();
                }
            }
        }, 300));
    }

    async render() {
        this.container.innerHTML = await this.#body();
        await this.#configure();
    }

    constructor(container) {
        this.container = container;
    }
}
