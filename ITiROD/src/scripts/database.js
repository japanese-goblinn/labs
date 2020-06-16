import Auth from "./auth.js";

export default class Database {

    static _showError(error) {
        alert(`Error. ${error.message}`);
    }
    
    static async search(string) {
        const searchString = string.toLowerCase();
        const results = [];
        const folders = await this.loadAllFolders();
        for (const folder of folders) {
            if (folder.title.toLowerCase().includes(searchString) || folder.description.toLowerCase().includes(searchString)) {
                results.push({
                    type: 'folder',
                    id: folder.id,
                    title: folder.title,
                    description: folder.description
                });
            }
            const notes = await this.loadAllNotes(folder.id);
            for (const note of notes) {
                if (!note.content.toLowerCase().includes(searchString)) {
                    continue;
                }
                results.push({
                    type: 'note',
                    id: note.id,
                    content: note.content,
                    folderID: folder.id
                });
            }
        }
        return Promise.all(results);
    }

    static async saveFolder(title, description) {
        const newFolderRef = this.db.ref('users/' + Auth.currentUser.uid + '/folders').push();
        newFolderRef.set({
            title: title,
            description: description
        });
        return newFolderRef.key;
    }

    static async deleteFolder(id) {
        const ref = this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + id);
        ref.remove().catch(this._showError);
    }

    static async editFolder(id, newTitle, newDescription) {
        const ref = this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + id);
        ref.update({
            title: newTitle,
            description: newDescription
        });
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

    static async saveNote(folderID, content, date) {
        const newNoteRef = this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + folderID + '/notes').push();
        newNoteRef.set({
            content: content,
            date: date
        });
        return newNoteRef.key;
    }

    static async deleteNote(folderID, noteID) {
        const ref = this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + folderID + '/notes/' + noteID);
        ref.remove().catch(this._showError);
    }

    static async moveNote(noteID, oldFolderID, newFolderID) {
        const note = await this.loadNote(oldFolderID, noteID);
        await this.saveNote(newFolderID, note.content, note.date);
        await this.deleteNote(oldFolderID, noteID);
    }

    static async loadNote(folderID, noteID) {
        const snapshot = await this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + folderID + '/notes/' + noteID).once('value');
        return {
            id: snapshot.key,
            folderID: folderID,
            content: snapshot.val().content,
            date: snapshot.val().date
        };
    }

    static async loadAllNotes(folderID) {
        const snapshot = await this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + folderID + '/notes').once('value');
        const notes = []
        snapshot.forEach((childSnap) => {
            notes.push({
                id: childSnap.key,
                content: childSnap.val().content,
                date: childSnap.val().date
            });
        });
        return Promise.all(notes);
    }

    static async updateNote(folderID, noteID, newContent) {
        const noteRef = this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + folderID + '/notes/' + noteID);
        noteRef.update({
            content: newContent
        });
    }

    static init() {
        this.db = firebase.database();
    }
}
