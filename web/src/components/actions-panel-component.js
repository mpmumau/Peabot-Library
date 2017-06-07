import React, {Component} from 'react';
import ActionsPanelSingleItemComponent from './actions-panel-single-item-component';
import SwipeableViews from 'react-swipeable-views'

import '../../scss/actions-panel-component.scss'

class ActionsPanelComponent extends Component {
    constructor(props) {
        super(props);

        this.captureMouse = this.captureMouse.bind(this);
    }

    captureMouse(e, v) {
        var screenX = e.screenX;
        var screenY = e.screenY;
        console.log("screenX: " + screenX + " screenY: " + screenY);
    }

    render() {
        return (
            <section className="actions-panel-component" onClick={(e, v) => this.captureMouse(e, v)}>
                <div className="actions-panel-wrapper">
                    <SwipeableViews>
                        <ActionsPanelSingleItemComponent iconType="heart" actionLabel="Love" actionName=""/>
                        <ActionsPanelSingleItemComponent iconType="resize-height" actionLabel="Elevate +" actionName="elevate_in"/>
                        <ActionsPanelSingleItemComponent iconType="resize-height" actionLabel="Elevate -" actionName="elevate_out"/>
                    </SwipeableViews>

                    <SwipeableViews>
                        <ActionsPanelSingleItemComponent iconType="resize-width" actionLabel="Extend +" actionName="extend_out"/>
                        <ActionsPanelSingleItemComponent iconType="resize-width" actionLabel="Extend -" actionName="extend_in"/>
                        <ActionsPanelSingleItemComponent iconType="musical-note" actionLabel="Sing"/>
                    </SwipeableViews>

                    <SwipeableViews>
                        <ActionsPanelSingleItemComponent iconType="beaker" actionLabel="Learn"/>
                    </SwipeableViews>
                </div>
            </section>
        );
    }
};

export default ActionsPanelComponent;