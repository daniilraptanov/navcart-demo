import { useDestinationStore } from '../../store/useDestinationStore';
import { useAppWebSocket } from '../../hooks/useWebSocket';
import { Arrow } from './Arrow';

export const Main = () => {
    const activeDestination = useDestinationStore((state) => state.getActiveDestination());
    const { isConnected } = useAppWebSocket();

    if (!activeDestination) {
        return (
            <main className="flex-1 flex flex-col items-center justify-center p-6">
                <div className="text-center">
                    <p className="text-2xl font-medium text-slate-400">
                        Choose destination
                    </p>
                </div>
            </main>
        );
    }

    return (
        <main className="flex-1 flex flex-col items-center justify-center p-6 w-full">
            
            {!isConnected && (
                <div className="text-center animate-fade-in">
                    <h2 className="text-2xl font-semibold text-slate-800 mb-2">
                        {activeDestination.name}
                    </h2>
                    <p className="text-slate-400">
                        NAVCART DEMO
                    </p>
                </div>
            )}

            {isConnected && (
                <div className="flex flex-col items-center justify-center w-full h-full animate-fade-in">
                    <h2 className="text-3xl font-bold text-slate-800 mb-8 text-center tracking-tight">
                        {activeDestination.name}
                    </h2>
                    
                    <Arrow />
                </div>
            )}
            
        </main>
    );
}
