import createAction from '../../middleware/actions';

async function getMonitor2({ getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.get("monitor2.json");
        if ( typeof obj == "string"){
            obj = JSON.parse(obj);
        }
        return {
            monitor2 : obj
        };
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(getMonitor2);