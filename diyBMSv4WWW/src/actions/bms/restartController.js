import createAction from '../../middleware/actions';

async function restartController(val , { getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.post("restartcontroller.json",val);
        return obj.success;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(restartController);