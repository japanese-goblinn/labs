export default class NotFounded {

    #body = async () => /*html*/`
        <h1>404 Not Founded</h1>
    `
    
    async render() {
        this.container.innerHTML = await this.#body();
    }

    constructor(container) {
        this.container = container;
    }
}
