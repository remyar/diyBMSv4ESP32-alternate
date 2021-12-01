import createAction from '../../middleware/actions';

async function getMonitor3({ getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.get("monitor3.json");
        return {
            monitor3 : obj
        };
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(getMonitor3);