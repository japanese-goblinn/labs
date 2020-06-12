import { router } from "../../app.js";

export default class NoteSearchComponent {

    #body = async (id, title) => /*html*/`
        <li id="sn-${id}" class="note-description selectable">
            <h4>${title}</h4>
            <p class="search-element-type secondary">Note</p>
        </li>
    `
    
    #configure = async () => {
        const element = document.getElementById(`sn-${this.id}`);
        element.addEventListener('click', async () => {
            await router.navigate('/');
            await router.navigate('folder/' + this.folderID);
            await router.navigate('note/' + this.id)
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
