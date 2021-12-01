import axios from 'axios';
import STATUS from '../status';

export default function get(url, isServiceWorker = false) {

    return axios.get( url).then((response)=>{
        const cookies = response.headers["set-cookie"];
        return response.data || {};
    }).catch((error)=>{
        throw error;
    });
}