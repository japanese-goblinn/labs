import { renderer } from './app.js';

export default class Router {

    #routes = {
        '/': 'MainPageView',
        '/welcome': 'WelcomePageView',
        '/folder/:id': 'NotesColumnView',
        '/folder/:id/note/:id': 'NoteView',
        '/404': '404'
    };

    _splitCurrentURL() {
        return this._splitURL(window.location.pathname);
    }

    _splitURL(url) {
        return url.split('/').filter(l => l);
    }

    _matchURL(splittedURL) {
        switch (splittedURL.length) {
            case 0:
                return '/';
            case 1:
                return '/' + splittedURL[0];
            case 2:
                return '/folder/:id';
            case 4:
                return '/folder/:id/note/:id';
            default:
                return '/404';
        }
    }

    _dataFromURL(splittedURL) {
        switch (splittedURL.length) {
            case 2:
                return {
                    folderID: splittedURL[1]
                };
            case 4:
                return {
                    folderID: splittedURL[1],
                    noteID: splittedURL[3]
                };
            default:
                return null;
        }
    }

    get currentView() {
        const path = this._matchURL(this._splitCurrentURL());
        return this.#routes[path];
    }

    async navigate(path) {
        const pathSplitted = this._splitURL(path);
        switch (pathSplitted[0]) {
            case 'folder':
                window.history.replaceState(null, null, '/');
                window.history.pushState(null, null, path);
                break;
            case 'note':
                const replace = this._splitCurrentURL().slice(0, 2).join('/');
                window.history.replaceState(null, null, '/');
                window.history.pushState(null, null, replace + '/');
                window.history.pushState(null, null, path);
                break;
            default:
                window.history.pushState(null, null, path);
        }
        await this.render();
    }

    async render() {
        const data = this._dataFromURL(this._splitCurrentURL());
        await renderer.render(this.currentView, data);
    }

    constructor() {
        window.history.replaceState(null, null, '/');
        window.addEventListener('popstate', async () => {
            await this.render();
        });
    }
}
