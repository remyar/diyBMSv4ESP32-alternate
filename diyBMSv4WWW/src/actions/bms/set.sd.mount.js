import createAction from '../../middleware/actions';

async function setSdMount( { getState, extra }) {
    let api = extra.api;
    try {
        let obj = await api.post("sdmount.json", { mount: 1 });
        return obj.success;
    }
    catch (err) {
        throw { message: err.message };
    }
}

export default createAction(setSdMount);