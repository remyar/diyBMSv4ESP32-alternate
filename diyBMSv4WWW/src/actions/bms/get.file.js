import createAction from '../../middleware/actions';

async function getFile(val , { getState , extra }) {

    let api = extra.api;
    let state = getState();
    try {
        let obj = await api.download("download?type=sdcard&file=" + encodeURI(val));
        return obj;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(getFile);