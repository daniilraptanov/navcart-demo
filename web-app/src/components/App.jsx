import { useDestinationStore } from '../store/useDestinationStore';
import { Header } from './Header';

export const App = () => {
    const activeDestination = useDestinationStore((state) => state.getActiveDestination());

    return (
        <div className="flex flex-col h-screen overflow-hidden bg-slate-50 text-slate-800 font-sans">            
            <Header />
            <main className="flex-1 flex flex-col items-center justify-center p-6">
                <div className="text-center">
                    <h2 className="text-2xl font-semibold text-slate-800 mb-2">
                        {activeDestination?.name}
                    </h2>
                    <p className="text-slate-400">
                        NAVCART DEMO
                    </p>
                </div>
            </main>

        </div>
    );
}
