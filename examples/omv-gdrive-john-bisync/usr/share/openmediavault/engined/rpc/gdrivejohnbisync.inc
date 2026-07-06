<?php

namespace Engined\Rpc;

class OMVRpcServiceGdrivejohnbisync extends \OMV\Rpc\ServiceAbstract
{
    public function getName()
    {
        return "gdrivejohnbisync";
    }

    public function initialize()
    {
        $this->registerMethod("getStatus");
        $this->registerMethod("getLog");
    }

    public function getStatus($params, $context)
    {
        $file = '/var/lib/openmediavault/gdrive-john-bisync/status.json';

        if (!file_exists($file)) {
            return [
                [
                    'status'  => '???',
                    'lastRun' => '???',
                    'message' => 'Aucun fichier de statut trouvé'
                ]
            ];
        }

        $data = json_decode(file_get_contents($file), true);

        if (!is_array($data)) {
            return [
                [
                    'status'  => '???',
                    'lastRun' => '???',
                    'message' => 'Impossible de lire status.json'
                ]
            ];
        }

        // Normalisation des valeurs
        if (empty($data['status']) || $data['status'] === 'unknown') {
            $data['status'] = '???';
        }

        if (empty($data['lastRun']) || $data['lastRun'] === 'unknown') {
            $data['lastRun'] = '???';
        }

        if (empty($data['message'])) {
            $data['message'] = 'Aucune synchronisation bisync n\'a encore été exécutée';
        }

        return [
            [
                'status'  => $data['status'],
                'lastRun' => $data['lastRun'],
                'message' => $data['message']
            ]
        ];
    }

    public function getLog($params, $context)
    {
        $file = '/var/log/omv-bisync-gdrive-john.log';

        if (!file_exists($file)) {
            return [
                ['line' => 'Log introuvable : ' . $file]
            ];
        }

        // Lecture du fichier ligne par ligne
        $lines = file($file, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);

        $result = [];
        foreach ($lines as $line) {
            $result[] = ['line' => $line];
        }

        return $result;
    }
}


