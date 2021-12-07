import createAction from '../../middleware/actions';

async function setStorage(val , { getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.post("savestorage.json",val);
        return obj.success;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(setStorage);