import React, {Component} from 'react';
import ActionsPanelSingleItemComponent from './actions-panel-single-item-component'

import '../../scss/actions-panel-component.scss'

class ActionsPanelComponent extends Component {
    render() {
        return (
            <section className='actions-panel-component'>
                <ActionsPanelSingleItemComponent iconType="heart" actionLabel="Love"/>
                <ActionsPanelSingleItemComponent iconType="resize-width" actionLabel="Extend"/>
                <ActionsPanelSingleItemComponent iconType="resize-height" actionLabel="Elevate"/>
                <ActionsPanelSingleItemComponent iconType="musical-note" actionLabel="Sing"/>
                <ActionsPanelSingleItemComponent iconType="beaker" actionLabel="Learn"/>
            </section>
        );
    }
};

export default ActionsPanelComponent;