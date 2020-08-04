export default function animate(button, animationName) {
    button.addEventListener('click', () => {
        button.classList.add(animationName);
    });
    button.addEventListener('animationend', () => {
        button.classList.remove(animationName);
    });
}
