import { router } from "../../app.js";

export default class FolderSearchComponent {

    #body = async (id, title, description) => /*html*/`
        <li id="sf-${id}" class="folder-description selectable">
            <h4>${title}</h4>
            <p class="secondary">${description}</p>
            <p class="search-element-type secondary">Folder</p>
        </li>
    `
    
    #configure = async () => {
        const element = document.getElementById(`sf-${this.id}`);
        element.addEventListener('click', async () => {
            await router.navigate('/');
            await router.navigate('folder/' + this.id);
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
        this.id = id;
        this.title = title;
        this.description = description;
    }
}
