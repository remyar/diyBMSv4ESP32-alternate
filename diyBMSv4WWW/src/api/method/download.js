import axios from 'axios';
import STATUS from '../status';

export default function download(url, isServiceWorker = false) {

    return axios.get(url, {
        responseType: 'blob',
    }).then((response) => {
        return response.data;
    }).catch((error) => {
        throw error;
    });
}