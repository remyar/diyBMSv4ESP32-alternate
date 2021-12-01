import createAction from '../../middleware/actions';

async function getRules({ getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.get("rules.json");
        return {
            rules : obj
        };
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(getRules);