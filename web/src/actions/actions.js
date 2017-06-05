export const testAction = (test_val) => {
    return {
        type: "TEST_ACTION",
        payload: test_val
    };
};

export const changePanelAction = (panel_name) => {
    return {
        type: "CHANGE_PANEL",
        payload: panel_name
    }
}