import { create } from 'zustand';

export const useDestinationStore = create((set, get) => ({
    destinations: [
        { id: 1, name: 'Молочний відділ' },
        { id: 2, name: 'Хліб' },
        { id: 3, name: 'Пиво' },
        { id: 4, name: 'Туалет' },
        { id: 5, name: 'Цукерки' },
        { id: 6, name: 'Напої' },
        { id: 7, name: 'М\'ясо' },
        { id: 8, name: 'Риба' },
    ],

    activeId: 0,
    
    setActiveId: (id) => set({ activeId: id }),
    getActiveDestination: () => {
        const state = get();
        return state.destinations.find(d => d.id === state.activeId);
    }
}));
