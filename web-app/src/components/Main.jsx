import { useDestinationStore } from '../store/useDestinationStore';

export const Main = () => {
    const activeDestination = useDestinationStore((state) => state.getActiveDestination());

    return (
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
    );
}
