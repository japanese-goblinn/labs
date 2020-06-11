import Auth from "./auth.js";

export default class Database {
    
    static async saveFolder(title, description) {
        const newFolderRef = this.db.ref('users/' + Auth.currentUser.uid + '/folders').push();
        newFolderRef.set({
            title: title,
            description: description
        });
        return newFolderRef.key;
    }

    static async loadFolder(id) {
        const snapshot = await this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + id).once('value');
        return {
            id: snapshot.key,
            title: snapshot.val().title,
            description: snapshot.val().description
        };
    }

    static async loadAllFolders() {
        const snapshot = await this.db.ref('users/' + Auth.currentUser.uid + '/folders').once('value');
        const folders = []
        snapshot.forEach((childSnap) => {
            folders.push({
                id: childSnap.key,
                title: childSnap.val().title,
                description: childSnap.val().description
            });
        });
        return Promise.all(folders);
    }

    static init() {
        this.db = firebase.database();
    }
}
