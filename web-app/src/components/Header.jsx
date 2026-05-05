import { useDestinationStore } from "../store/useDestinationStore";

export const Header = () => {
    const destinations = useDestinationStore((state) => state.destinations);
    const activeId = useDestinationStore((state) => state.activeId);
    const setActiveId = useDestinationStore((state) => state.setActiveId);
    
    return (
        <header className="bg-white shadow-sm p-4 z-10">
            <div className="flex gap-3 overflow-x-auto scrollbar-hide">
                {destinations.map((pos) => {
                    const isActive = activeId === pos.id;
                    return (
                        <div
                            key={pos.id}
                            onClick={() => setActiveId(pos.id)}
                            className={`
                                whitespace-nowrap px-5 py-2.5 rounded-xl text-[15px] font-medium 
                                cursor-pointer transition-all duration-200 select-none
                                ${isActive 
                                    ? 'bg-blue-500 text-white shadow-lg shadow-blue-500/30' 
                                    : 'bg-slate-100 text-slate-600 hover:bg-slate-200'
                                }
                            `}
                        >
                            {pos.name}
                        </div>
                    );
                })}
            </div>
        </header>
    )
}