import 'isomorphic-fetch';
import STATUS from '../status';

export default function post(url, data = undefined) {

    let headers = {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
    };

    return fetch(url, {

        headers: new Headers(headers),
        method: 'POST',
        body: JSON.stringify(data),

    }).then((res) => {
        if (res.status === STATUS.NO_CONTENT || (res?.headers?.get('Content-Length') === "0")) {
            return true;
        } else {
            return res.json();
        }

    }).then((res) => {

        if (res.errorCode !== undefined || res.error !== undefined) {
            res.errorCode = res.errorCode ? res.errorCode : res.error;
            throw res;
        } else {
            return res;
        }

    }).catch((err) => {

        return Promise.reject(STATUS.mdw(err));

    });
}
