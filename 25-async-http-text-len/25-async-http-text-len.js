#!/usr/bin/env node

async function http_get(url) {
    try {
        const res = await fetch(url);
        if (!res.ok) return null;
        return await res.text();
    } catch {
        return null;
    }
}

const urls = [
    'https://jsonplaceholder.typicode.com/posts/1',
    'https://http.codes/200',
];

function http_lengths(urls) {
    return Promise.all(urls.map(url => http_get(url).then(text => (text?.length ?? null))));
}

(async function() {
    (await http_lengths(urls)).forEach(n => console.log(n));
})()
