import createAction from '../../middleware/actions';

async function getStorage({ getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.get("storage.json");
        return {storage : obj};
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(getStorage);