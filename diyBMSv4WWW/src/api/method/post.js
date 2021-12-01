import axios from 'axios';

export default function post(url, val, isServiceWorker = false) {

    var bodyFormData = new FormData();

    Object.keys(val).map((key)=>{
        bodyFormData.append(key , val[key]);
    });

    return axios({
        method: "post",
        url: url,
        data: bodyFormData,
        headers: { "Content-Type": "multipart/form-data" }
    })
    .then(function (response) {
        return response.data || {};
    })
    .catch(function (error) {
        throw error;
    });
}