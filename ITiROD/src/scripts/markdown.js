export default function handleMarkdown(inputSource, previewSource) {
    inputSource.addEventListener('input', () => {
        previewSource.innerHTML = marked(inputSource.value);
    });
}
