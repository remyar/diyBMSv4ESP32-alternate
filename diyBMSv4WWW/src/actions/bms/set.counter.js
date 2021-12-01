import createAction from '../../middleware/actions';

async function setCounter(val , { getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.post("resetcounters.json",val);
        return obj.success;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(setCounter);