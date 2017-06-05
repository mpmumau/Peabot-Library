export default function (state=null, action) {
    switch(action.type) {
        case "FUCKAH_EVENT":
            return action.payload;
            break;
    }
    return state;
};