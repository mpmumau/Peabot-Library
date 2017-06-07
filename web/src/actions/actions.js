export const changePanelAction = (panel_name) => {
    return {
        type: "CHANGE_PANEL",
        payload: panel_name
    }
}

export const changeSpeed = (speed) => {
    return {
        type: "CHANGE_SPEED",
        payload: speed
    }
}